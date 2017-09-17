from django.http import HttpResponse, HttpResponseRedirect
from django.template import RequestContext, loader, Template
from django.core.urlresolvers import reverse
from django.shortcuts import render, get_object_or_404

from .models import Choice, Poll

def index(request):
    latest_poll_list = Poll.objects.all().order_by('-pub_date')[:5]
    context = {'latest_poll_list': latest_poll_list[1:3]}
    return render(request, 'polls/index.html', context)

def detail(request, poll_id):
    if int(poll_id) > 3:
        return render(request, 'polls/detail.html', {'poll': {"id": int(poll_id), "question": "ahhhhh"}})
    poll = get_object_or_404(Poll, pk=poll_id)
    return render(request, 'polls/detail.html', {'poll': poll})

def results(request, poll_id):
    poll = get_object_or_404(Poll, pk=poll_id)
    return render(request, 'polls/results.html', {'poll': poll})

def vote(request, poll_id):
    p = get_object_or_404(Poll, pk=poll_id)
    return render(request, 'polls/detail.html', {
        'poll': p,
        'error_message': "You are unable to vote at this time.",
    })
