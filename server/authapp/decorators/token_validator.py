import base64
import datetime
from functools import wraps
import hashlib
from typing import Dict
from django.http import HttpRequest, JsonResponse
import jwt
from django.conf import settings

from authapp.models import UserRefreshToken
from authapp.models import User

#! Unoptimized
def validate_access_token():
    def decorator(view_func):
        @wraps(view_func)
        def _wrapped_view(request, *args, **kwargs):
            try:
                JWT_PUBLIC_KEY = getattr(settings, 'JWT_PUBLIC_KEY', None)
                token = validate_header_format(request)
                obj = get_token_object(token, JWT_PUBLIC_KEY)
                refresh_token_hash = base64.b64decode(obj['refresh_token_fingerprint'])
                validate_refresh_token_hash(refresh_token_hash)
                validate_token_lifetime(obj)
                request.decoded_token = obj
            except UnauthException as e:
                return JsonResponse({'error': e.message}, status=401)
            return view_func(request, *args, **kwargs)
        return _wrapped_view
    return decorator

def validate_refresh_token():
    def decorator(view_func):
        @wraps(view_func)
        def _wrapped_view(request: HttpRequest, *args, **kwargs):
            # Extract the token from the Authorization header
            try:
                JWT_PUBLIC_KEY = getattr(settings, 'JWT_PUBLIC_KEY', None)
                token = validate_header_format(request)
                obj = get_token_object(token, JWT_PUBLIC_KEY)
                refresh_token_hash = hashlib.sha512(token.encode('utf-8')).digest()
                validate_refresh_token_hash(refresh_token_hash)
                validate_token_lifetime(obj)
                request.decoded_token = obj
            except UnauthException as e:
                return JsonResponse({'error': e.message}, status=401)
            return view_func(request, *args, **kwargs)
        return _wrapped_view
    return decorator

def retrieve_user_from_access_token():
    def decorator(view_func):
        @wraps(view_func)
        def _wrapped_view(request, *args, **kwargs):
            try:
                access_token = request.decoded_token
                string_identifier = access_token['identifier']
                if access_token['role'] == 'common' or access_token['role'] == 'company':
                    binary_identifier = base64.b64decode(string_identifier)
                    user = User.objects.get(telephone_hash=binary_identifier)
                    request.user = user
                else:
                    return JsonResponse({'error': 'user_only_route'}, status=403)
            except KeyError as e:
                return view_func(request, *args, **kwargs)
            return view_func(request, *args, **kwargs)
        return _wrapped_view
    return decorator

def validate_header_format(request: HttpRequest):
    token = request.META.get('HTTP_AUTHORIZATION')
    if not token or not token.startswith('Bearer '):
        raise UnauthException("Invalid header format, expected \"Bearer <token>\"")

    return token.split(' ')[1]

def get_token_object(token: str, jwt_public_key: str):
    try:
        return jwt.decode(token, key=jwt_public_key, algorithms=['ES512'])
    except (jwt.InvalidSignatureError, jwt.DecodeError):
        raise UnauthException('Corrupted Token')

def validate_refresh_token_hash(token_hash: bytes):    
    try:
        UserRefreshToken.objects.get(refresh_token_hash=token_hash)
        return
    except UserRefreshToken.DoesNotExist:
        raise UnauthException('Token Not Registered')
    

def validate_token_lifetime(obj: Dict):
    expiration = datetime.datetime.strptime(obj['expiration'], "%d/%m/%Y, %H:%M:%S, %z")
    if datetime.datetime.now().astimezone() > expiration.astimezone():
        raise UnauthException('Token Has Expired')

class UnauthException(Exception):
    def __init__(self, message):
        self.message = message
        super().__init__(self.message)