import json
from django.conf import settings
from django.views.decorators.http import require_http_methods
from django.http import HttpRequest, HttpResponse, HttpResponseRedirect, JsonResponse
from django.views.decorators.csrf import csrf_exempt

from Shared.token_gen import gen_token_redirect_url, gen_tokens
from authapp.models.user import User
from authapp.views.Shared.user import register_user
from .Shared.github import get_github_user, register_github_info

@csrf_exempt
@require_http_methods(['GET'])
def login_github(request: HttpRequest):
    try:
        query_params = request.GET.dict()
        extra_data = json.loads(query_params['state'])
        user_data = get_github_user(query_params)
        user_info : User = User.objects.get(name = user_data['login'])
    except User.DoesNotExist:
        username = user_data['email'].split("@")[0]
        user_info = register_user(username, User.UserSubscription.GITHUB)
        register_github_info(user_info, f"{user_data['id']}")
    return HttpResponseRedirect(redirect_to=gen_token_redirect_url(gen_tokens(user_info), extra_data['protocol']))