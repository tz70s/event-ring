#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 5
#define READ_SIZE 10

int main(void)
{
    /* Pre-defiend flags. */
    int running = 1, event_count, i;
    char read_buffer[READ_SIZE + 1];
    unsigned long result = 0;
    int event_fd = eventfd(0, 0);

    printf("Event fd num : %d\n", event_fd);

    struct epoll_event event, events[MAX_EVENTS];
    int epoll_fd = epoll_create1(0);

    if (epoll_fd < 0)
    {
        fprintf(stderr, "Failed to create epoll fd\n");
        return 1;
    }

    event.events = EPOLLIN;
    event.data.fd = event_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event))
    {
        fprintf(stderr, "Failed to add file descripter to epoll\n");
        close(epoll_fd);
        return 1;
    }

    while (running)
    {
        printf("\nPolling for input, fd0...\n");
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
        printf("%d ready events\n", event_count);
        for (i = 0; i < event_count; i++)
        {
            printf("Reading file descriptor '%d' -- ", events[i].data.fd);
            /* Read data from event fd. */
            if (read(events[i].data.fd, &result, sizeof(result)) < 0)
            {
                goto out;
            }
            printf("read result %lu\n", result);

            if (!strncmp(read_buffer, "stop\n", 5))
                running = 0;
        }
    }

out:
    if (close(epoll_fd))
    {
        close(event_fd);
        fprintf(stderr, "Failed to close epoll fd\n");
        return 1;
    }

    return 0;
}
