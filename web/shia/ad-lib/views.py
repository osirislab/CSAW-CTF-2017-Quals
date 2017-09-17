from django.http import HttpResponse, HttpResponseRedirect
from django.template import RequestContext, loader, Template
from django.core.urlresolvers import reverse
from django.shortcuts import render, get_object_or_404

from someclass import Woohoo

obj = Woohoo()
TEMP = """
{{% extends 'base.html' %}}
{{% load pools_extras %}}

{{% block breadcrumb-page %}}Format{{% endblock breadcrumb-page %}}
{{% block content %}}
{}
{{% endblock content %}}
"""

def index(request):
    global obj

    if request.method == "POST":
        data = request.POST.get('formatdata', '')
        template_data = TEMP.format(data.replace("noun", "noun|safe").replace("verb", "verb|safe").replace("adjective", "adjective|safe"))
        template = Template(template_data)
        context = RequestContext(request, {
            'noun': '<img src="https://media0.giphy.com/media/arNexgslLkqVq/200.webp#70-grid1" />',
            'verb': '<img src="https://media3.giphy.com/media/R0vQH2T9T4zza/200.webp#165-grid1" />',
            'adjective': '<img src="https://media1.giphy.com/media/TxXhUgEUWWL6/200.webp#129-grid1" />',
            'mrpoopy': obj
        })
        shit = template.render(context)
        return HttpResponse(shit)
    return render(request, 'ad-lib/format.html')
