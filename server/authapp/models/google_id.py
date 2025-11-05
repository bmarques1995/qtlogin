from django.db import models
from authapp.fields.byte_stream import ByteStreamField
from .user import User

class GoogleID(models.Model):
    user_idx = models.OneToOneField(to=User, on_delete=models.CASCADE)
    google_id_hash = ByteStreamField(stream_size=60, null=False)