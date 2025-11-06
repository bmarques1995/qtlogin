import json
from django.conf import settings
import requests
from requests.exceptions import RequestException
from urllib import parse as url_parser

from Shared.domain_exception import DomainException

def get_github_oauth_token(query_params: dict):
    url: str = "https://github.com/login/oauth/access_token"

    values: dict = {
        'client_id': getattr(settings, 'GITHUB_CLIENT_ID', None),
        'client_secret': getattr(settings, 'GITHUB_CLIENT_SECRET', None),
        'code': query_params['code']
    }

    encoded_params = url_parser.urlencode(values)
    full_url = f"{url}?{encoded_params}"

    try:
        token_response = requests.get(url= full_url, headers= {}).content.decode()
        return url_parser.parse_qs(token_response)
    except (RequestException) as e:
        raise DomainException("Invalid Google Key")
    
def retrieve_github_user_info(access_token: str):
    url: str = f"https://api.github.com/user"
    try:
        user_response = requests.get(url= url, headers= {
            'Authorization': f"Bearer {access_token}",
        }).content.decode()
        return json.loads(user_response)
    except Exception:
        raise DomainException("Invalid Google Key")
