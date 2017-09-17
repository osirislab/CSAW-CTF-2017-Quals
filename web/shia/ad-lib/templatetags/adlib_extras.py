from django import template

register = template.Library()

# Cause error in filter to disclose source code?

@register.filter(name='dowrite')
def dowrite(value, arg):
  with open(value, "w+") as f:
    f.write(arg)
  return
