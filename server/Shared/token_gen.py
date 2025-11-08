from datetime import datetime
from typing import Tuple


import jwt
from authapp.models import User
from django.conf import settings
import hashlib
import base64

from authapp.serializers import UserRefreshTokenSerializer
from Shared.domain_exception import DomainException
from django.core.cache import cache

time_pattern = "%d/%m/%Y, %H:%M:%S, %z"

def gen_tokens(user_data: User):
    JWT_PRIVATE_KEY = getattr(settings, 'JWT_PRIVATE_KEY', None)
    
    refresh_token, refresh_token_expiration = gen_refresh_token(user_data, JWT_PRIVATE_KEY)
    refresh_token_hash = hashlib.sha512(refresh_token.encode('utf-8')).digest()

    try:
        save_token_hash(user_data, refresh_token_hash, refresh_token_expiration)
    except DomainException:
        return {
            'refresh_token': "I'm a dumb, the data is mismatching",
            'access_token': "The data is mismatching",
        }

    return {
        'refresh_token': refresh_token,
        'access_token': gen_access_token(user_data, refresh_token_hash, JWT_PRIVATE_KEY),
    }

def gen_refresh_token(user_data: User, jwt_private_key) -> Tuple[str, datetime]:
    refresh_token_expiration = (datetime.now() + getattr(settings, 'REFRESH_TOKEN_LIFETIME', None))
    identifier = user_data.name

    refresh_token_info = {
        'identifier': identifier,
        'expiration': refresh_token_expiration.astimezone().strftime(time_pattern)
    }

    token = jwt.encode(payload= refresh_token_info, key=jwt_private_key, algorithm="ES512")

    refresh_token_cache_timeout = settings.REFRESH_TOKEN_CACHE_TIMEOUT
    cache.set(f"refresh_token_{identifier}", token, timeout=refresh_token_cache_timeout)

    return token, refresh_token_expiration

def save_token_hash(user_data: User, refresh_token_hash: bytes, refresh_token_expiration: datetime) -> None:
    treated_data = {
        'user_idx': user_data.name,
        'refresh_token_hash': refresh_token_hash,
        'expiration': refresh_token_expiration
    }

    try:    
        refresh_token_serializer = UserRefreshTokenSerializer(data= treated_data)
        refresh_token_serializer.is_valid(raise_exception=True)
        refresh_token_serializer.save()
    except Exception:
        raise DomainException("Incorrect data")
    
def gen_access_token(user_data: User, refresh_token_hash: bytes, jwt_private_key):

    identifier = user_data.name

    access_token_info = {
        # temp, 
        'identifier': identifier,
        'expiration': (datetime.now() + getattr(settings, 'ACCESS_TOKEN_LIFETIME', None)).astimezone().strftime(time_pattern),
        'refresh_token_fingerprint': base64.b64encode(refresh_token_hash).decode()
    }
    
    token = jwt.encode(access_token_info, key=jwt_private_key, algorithm="ES512")
    #cache token (10s)
    #key = access_token_<role>_<identifier>
    access_token_cache_timeout = settings.ACCESS_TOKEN_CACHE_TIMEOUT
    cache.set(f"access_token_{identifier}", token, timeout=access_token_cache_timeout)

    return token

def get_token_object(header_token: str):
    token = header_token.split(' ')[1]
    JWT_PUBLIC_KEY = getattr(settings, 'JWT_PUBLIC_KEY', None)
    return jwt.decode(token, key=JWT_PUBLIC_KEY, algorithms=['ES512'])

def gen_token_redirect_url(tokens: dict, protocol):
    base_front_url = getattr(settings, 'FRONTEND_ROUTER_BASE_ROUTE', None)
    token_path = getattr(settings, 'FRONTEND_ROUTER_TOKEN_REDIRECT', None)
    return f"{base_front_url}/{token_path}/{protocol}/{tokens['refresh_token']}/{tokens['access_token']}"