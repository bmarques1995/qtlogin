from django.db import models
from authapp.fields.byte_stream import ByteStreamField
from .user import User

class GithubID(models.Model):
    user_idx = models.OneToOneField(to=User, on_delete=models.CASCADE)
    github_id_hash = ByteStreamField(stream_size=60, null=False)