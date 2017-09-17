FROM gcr.io/google-appengine/debian8
RUN rm -rf /app/ && mkdir /app/
RUN apt-get update && apt-get install -y gcc-multilib build-essential apt-utils upx gdb
COPY src/* /app/
RUN cd /app/ && make && strip minesweeper && upx minesweeper
RUN echo "cd /app && ./minesweeper" > /app/run.sh
RUN chmod -R 700 /app/
RUN echo "flag{h3aps4r3fun351eabf3}" > /app/flag
RUN chmod a+rx /app /app/minesweeper /app/run.sh /app/flag
RUN useradd minesweeper
EXPOSE 31337
USER minesweeper
#ENTRYPOINT sh -c "/bin/bash"
ENTRYPOINT bash /app/run.sh
