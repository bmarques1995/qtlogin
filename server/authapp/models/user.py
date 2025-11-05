from django.db import models
from django.conf import settings
from django_mysql.models import EnumField

class User(models.Model):
    class UserSubscription(models.TextChoices):
        OWN = "own"
        GOOGLE = "google"
        GITHUB = "github"

    name = models.CharField(max_length=255, primary_key=True, null=False)
    email = models.CharField(max_length=255, unique=True, null=False)
    subscription = EnumField(choices=UserSubscription.choices, default=UserSubscription.OWN)
    confirmed_registration = models.BooleanField(default=False, null=False)
    account_active = models.BooleanField(default=True, null=False)
    creation_date = models.DateTimeField(auto_now_add=True, null=False)
