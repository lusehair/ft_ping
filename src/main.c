
# include "../inc/ft_ping.h"

t_ping g_ping;

void sigint_handler(int signo) {
    if (signo == SIGINT) {
        printf("\n--- %s ping statistics ---\n", g_ping.target);
        printf("%d packets transmitted, %d received, %d%% packet loss, time %ldms\n", g_ping.seq, g_ping.received, (int)((float)(g_ping.seq - g_ping.received) / g_ping.seq) * 100, g_ping.total_time);
        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", g_ping.min_rtt, g_ping.sum_rtt / g_ping.received, g_ping.max_rtt, g_ping.mdev_rtt);
        close(g_ping.sockfd);
        exit(0);
    }
}

int main(int ac, char **av) {

	char *arg;
	int i = 1;
	int j = 1;

	if (ac < 2) {
		printf("Usage: ping [-hv] destination\n");
		return (1);
	}

	while (i < ac) {
		arg = av[i];
		if (arg[0] == '-') {
			if (strlen(arg) < 2) {
				printf("ping: -: Name or service not known\n");
				return (1);
			}
			while (arg[j] != '\0') {
				if (arg[j] == 'h') {
				printf("Usage: ping [-hv] destination\n");
					return (1);
				}
				else if (arg[j] == 'v')
					g_ping.v_option = 1;
				else {
					printf("ping: option requires an argument -- '%c'\n", arg[j]);
					printf("Usage: ping [-hv] destination\n");
					return (1);
				}
				j++;
			}
		}
		else {
			snprintf(g_ping.target, sizeof(g_ping.target), "%s", arg);
			parse_hostname(arg);
		}
		i++;
	}
	signal(SIGINT, sigint_handler);
	init_socket();
}