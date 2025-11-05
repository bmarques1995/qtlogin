from django.urls import path
from rest_framework.routers import DefaultRouter
from authapp.views import login_github, login_google, sample_route

urlpatterns = [
    path(route='sample_route', view=sample_route, name='sample_route'),
    path(route='login_google', view=login_google, name='login_google'),
    path(route='login_github', view=login_github, name='login_github'),
]