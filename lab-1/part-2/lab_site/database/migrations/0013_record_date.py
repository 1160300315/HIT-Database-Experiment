# Generated by Django 2.2 on 2019-04-19 07:59

import datetime
from django.db import migrations, models
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('database', '0012_auto_20190419_0754'),
    ]

    operations = [
        migrations.AddField(
            model_name='record',
            name='date',
            field=models.DateTimeField(default=datetime.datetime(2019, 4, 19, 7, 59, 7, 343506, tzinfo=utc)),
            preserve_default=False,
        ),
    ]
