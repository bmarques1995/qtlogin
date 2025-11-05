from django.conf import settings
from django.views.decorators.http import require_http_methods
from django.http import HttpRequest, HttpResponse, JsonResponse
from django.views.decorators.csrf import csrf_exempt
from Shared.google_auth import get_google_oauth_token, get_google_user

@csrf_exempt
@require_http_methods(['GET'])
def login_google(request: HttpRequest):
    query_params = request.GET.dict()
    token_data = get_google_oauth_token(query_params, settings.GOOGLE_REDIRECT_URI_LOGIN)
    #return JsonResponse(token_data)
    user_data = get_google_user(token_data['id_token'], token_data['access_token'])
    return JsonResponse(user_data, status=200)