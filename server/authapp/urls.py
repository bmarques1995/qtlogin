from django.urls import path
from rest_framework.routers import DefaultRouter
from authapp.views import login_github, login_google, sample_public_route, sample_private_route

urlpatterns = [
    path(route='sample_public_route', view=sample_public_route, name='sample_public_route'),
    path(route='sample_private_route', view=sample_private_route, name='sample_private_route'),
    path(route='login_google', view=login_google, name='login_google'),
    path(route='login_github', view=login_github, name='login_github'),
]