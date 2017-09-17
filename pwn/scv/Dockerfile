FROM ubuntu:16.04
MAINTAINER unknonwn
LABEL Description="CSAW 2017 SCV" VERSION='1.0'

#installation
RUN apt-get update && apt-get upgrade -y 
RUN apt-get install -y socat build-essential

#user
RUN adduser --disabled-password --gecos '' scv
RUN chown -R root:scv /home/scv/
RUN chmod 750 /home/scv
RUN chmod 740 /usr/bin/top
RUN chmod 740 /bin/ps
RUN chmod 740 /usr/bin/pgrep
RUN export TERM=xterm

WORKDIR /home/scv/

COPY scv.cpp /home/scv
COPY flag /home/scv

RUN chown root:scv /home/scv/flag
RUN chmod 440 /home/scv/flag
RUN g++ -Wall -o scv scv.cpp
RUN strip -s scv
RUN rm scv.cpp

EXPOSE 8025
CMD su scv -c "socat -T10 TCP-LISTEN:8025,reuseaddr,fork EXEC:/home/scv/scv"
 
