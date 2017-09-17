from django.conf.urls import include, url
from django.views.generic import TemplateView

from django.contrib import admin
admin.autodiscover()

urlpatterns = [
    url(r'^$', TemplateView.as_view(template_name="index.html"), name='home'),
    url(r'^polls/', include('polls.urls', namespace="polls")),
    url(r'^ad-lib/', include('ad-lib.urls', namespace="ad-lib")),
    #url(r'^admin/', include(admin.site.urls)),
]
