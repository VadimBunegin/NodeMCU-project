from django.contrib.auth.models import User
from django.db import models


class History(models.Model):
    date = models.DateTimeField(default=0)
    first = models.IntegerField(default=0)
    second = models.FloatField(default=0)
    result = models.IntegerField(default=0)
