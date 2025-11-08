from django.conf import settings

from Shared.crypt import gen_password
from Shared.google_auth import get_google_oauth_token, retrieve_google_user_info
from authapp.serializers.google_id import GoogleIDSerializer

def get_google_user(query_params):
    token_data = get_google_oauth_token(query_params, settings.GOOGLE_REDIRECT_URI_LOGIN)
    return retrieve_google_user_info(token_data['id_token'], token_data['access_token'])

def register_google_info(user_info, id):
    hashed_id = gen_password(id, rounds=11)
    cnpj_obj = {
        'user_idx': user_info.name,
        'google_id_hash': hashed_id
    }
    google_id_serializer = GoogleIDSerializer(data = cnpj_obj)
    if google_id_serializer.is_valid():
        google_id_serializer.save()
