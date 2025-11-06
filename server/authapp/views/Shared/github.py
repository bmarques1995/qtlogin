from Shared.github_auth import get_github_oauth_token, retrieve_github_user_info


def get_github_user(query_params):
    token_data = get_github_oauth_token(query_params)
    #return JsonResponse(token_data)
    return retrieve_github_user_info(token_data['access_token'][0])