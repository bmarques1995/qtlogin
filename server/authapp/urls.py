from django.urls import path
from rest_framework.routers import DefaultRouter
from authapp.views import sample_route

urlpatterns = [
    path(route='sample_route', view=sample_route, name='sample_route')
]