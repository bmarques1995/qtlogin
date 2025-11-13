import base64
import hashlib
from django.conf import settings
from django.http import HttpRequest, JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import require_http_methods
import jwt
from rest_framework.exceptions import ValidationError

from authapp.decorators.body_validation import validate_request_json_body
from authapp.decorators.token_validator import validate_refresh_token

from authapp.models import User, UserRefreshToken
from django.core.cache import cache

from Shared.token_gen import gen_access_token, gen_tokens, get_token_object

JWT_PRIVATE_KEY = getattr(settings, 'JWT_PRIVATE_KEY', None)

@csrf_exempt
@require_http_methods(['POST'])
@validate_refresh_token()
def renew_access_token(request: HttpRequest): 
    token_str = request.META.get('HTTP_AUTHORIZATION').split(' ')[1]
    token = request.decoded_token
    refresh_token_hash = hashlib.sha512(token_str.encode('utf-8')).digest()

    user_data = User.objects.get(name=token['identifier'])
    refresh_token = UserRefreshToken.objects.get(refresh_token_hash= refresh_token_hash)
    refresh_token.delete()
    tokens = gen_tokens(user_data)
    
    return JsonResponse(tokens,status=200)

@csrf_exempt
@require_http_methods(['POST'])
@validate_refresh_token()
def revoke_refresh_token(request: HttpRequest):
    token = request.decoded_token
    token_str = request.META.get('HTTP_AUTHORIZATION').split(' ')[1]
    refresh_token_hash = hashlib.sha512(token_str.encode('utf-8')).digest()
    
    user_data = User.objects.get(name=token['identifier'])
    refresh_token = UserRefreshToken.objects.get(refresh_token_hash= refresh_token_hash)
    refresh_token.delete()
    return JsonResponse({'Message': "The provided token is revoked"},status=200)

def retrieve_final_access_token(token, refresh_token_hash):
    user_data = User.objects.get(telephone_hash=base64.b64decode(token['identifier']))
    
    cached_token = cache.get(f"refresh_token_{token['identifier']}")
    if cached_token is not None:
        return cached_token
    else:
        return gen_access_token(user_data, refresh_token_hash, JWT_PRIVATE_KEY)
