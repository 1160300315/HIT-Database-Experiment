from django.shortcuts import render
from django.urls import reverse
from django.http import HttpResponseRedirect


def welcome(request):
    return render(request, "welcome.html")


def visit_admin(request):
    return HttpResponseRedirect("/admin")


def visit_submit(request):
    return HttpResponseRedirect(reverse("database:submit"))


def visit_detail(request):
    return HttpResponseRedirect(reverse("database:detail"))
