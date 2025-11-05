import json
from django.conf import settings
import requests
from requests.exceptions import RequestException
from urllib import parse as url_parser

from Shared.domain_exception import DomainException

def get_google_oauth_token(query_params: dict, redirect_uri: str = None):
    url: str = "https://oauth2.googleapis.com/token"

    values: dict = {
        'code': query_params['code'],
        'client_id': getattr(settings, 'GOOGLE_CLIENT_ID', None),
        'client_secret': getattr(settings, 'GOOGLE_CLIENT_SECRET', None),
        'redirect_uri': redirect_uri,
        'grant_type': "authorization_code",
    }

    try:
        token_response = requests.post(url= url, data = url_parser.urlencode(values),headers= {
            "Content-Type": "application/x-www-form-urlencoded", 
        }).content.decode()
        return json.loads(token_response)
    except (RequestException) as e:
        raise DomainException("Invalid Google Key")
    
def get_google_user(id_token: str, access_token: str):
    url: str = f"https://www.googleapis.com/oauth2/v1/userinfo?alt=json&access_token={access_token}"
    try:
        user_response = requests.get(url= url, headers= {
            'Authorization': f"Bearer {id_token}",
        }).content.decode()
        return json.loads(user_response)
    except Exception:
        raise DomainException("Invalid Google Key")
