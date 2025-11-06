from django.conf import settings

from Shared.google_auth import get_google_oauth_token, retrieve_google_user_info

def get_google_user(query_params):
    token_data = get_google_oauth_token(query_params, settings.GOOGLE_REDIRECT_URI_LOGIN)
    return retrieve_google_user_info(token_data['id_token'], token_data['access_token'])