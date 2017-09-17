FROM ubuntu:16.04
MAINTAINER unknonwn
LABEL Description="CSAW 2017 AUIR" VERSION='1.0'

#installation
RUN apt-get update && apt-get upgrade -y 
RUN apt-get install -y socat build-essential

#user
RUN adduser --disabled-password --gecos '' auir
RUN chown -R root:auir /home/auir/
RUN chmod 750 /home/auir
RUN chmod 740 /usr/bin/top
RUN chmod 740 /bin/ps
RUN chmod 740 /usr/bin/pgrep
RUN export TERM=xterm

WORKDIR /home/auir/

COPY auir /home/auir
COPY flag /home/auir

RUN chown root:auir /home/auir/flag
RUN chmod 440 /home/auir/flag

RUN strip -s auir

EXPOSE 8026
CMD su auir -c "socat -T10 TCP-LISTEN:8026,reuseaddr,fork EXEC:/home/auir/auir"
 
