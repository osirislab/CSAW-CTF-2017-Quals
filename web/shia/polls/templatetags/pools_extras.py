import re
from django import template

register = template.Library()

@register.filter(name='getme')
def getme(value, arg):
  return getattr(value, arg)

@register.filter(name='checknum')
def checknum(value):
  check(value)

@register.filter(name='listme')
def listme(value):
  return dir(value)

def check(value):
  if value > 2:
    raise Exception("Our infrastructure can't support that many Shias!")
