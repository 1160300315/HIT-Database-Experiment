# Generated by Django 2.2 on 2019-04-19 06:16

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('database', '0009_auto_20190418_1843'),
    ]

    operations = [
        migrations.RenameField(
            model_name='broughtin',
            old_name='begin_date',
            new_name='date',
        ),
    ]
