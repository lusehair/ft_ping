# include "ft_ping.h"

extern t_ping g_ping;

unsigned short checksum(unsigned short *packet, int buffer_size) {
	unsigned long sum = 0;
	unsigned short result;

	while (buffer_size > 1) {
		sum += *packet;
		packet++;
		buffer_size -= 2;
	}

	if (buffer_size == 1) {
		unsigned short lastByte = *(unsigned char *)packet;
		sum += lastByte;
	}

	while (sum >> 16) {
		sum = (sum & 0xFFFF) + (sum >> 16);
	}

	result = ~sum;
	return result;
}

void ping_loop() {
    struct msghdr msg = {0};
    struct iovec iov = {0};
    struct icmp icmp = {0};
    struct timeval send_time, recv_time;
    int seq_sum = 1, icmp_packet_size = PACKET_SIZE + sizeof(struct icmphdr);
    int total_packet_size = sizeof(struct ip) + icmp_packet_size;
    char packet[total_packet_size];

    while (1) {
        bzero(&icmp, sizeof(icmp));
        icmp.icmp_type = ICMP_ECHO;
        icmp.icmp_id = getpid();
        icmp.icmp_seq = seq_sum++;
        gettimeofday(&send_time, NULL);
        memcpy(icmp.icmp_data, &send_time, sizeof(struct timeval));
        if (seq_sum == 2)
            printf("PING %s (%s) %d(%d) bytes of data.\n", g_ping.target, g_ping.ip, PACKET_SIZE, total_packet_size);
        icmp.icmp_cksum = checksum((unsigned short *)&icmp, sizeof(icmp));
        iov.iov_base = packet; iov.iov_len = total_packet_size;
        msg.msg_iov = &iov; msg.msg_iovlen = 1;

        if (sendto(g_ping.sockfd, &icmp, sizeof(icmp), 0, (struct sockaddr *)g_ping.res, sizeof(struct sockaddr)) < 0) {
            perror("sendto");
            if (g_ping.v_option == 0) { close(g_ping.sockfd); exit(1); }
        }
        g_ping.seq++;
        int nbytes = recvmsg(g_ping.sockfd, &msg, 0);
        if (nbytes < 0) {
            if (errno == EAGAIN) printf("ft_ping: sendmsg: Network is unreachable\n");
            else perror("recvfrom");
            continue;
        }

        gettimeofday(&recv_time, NULL);
        struct ip *ip = (struct ip *)packet;
        struct icmp *icmp_rec = (struct icmp *)(packet + (ip->ip_hl << 2));
        struct timeval *out = (struct timeval *)(icmp_rec->icmp_data);
        double latency = (double)(recv_time.tv_sec - out->tv_sec) * 1000.0 + (double)(recv_time.tv_usec - out->tv_usec) / 1000.0;
        printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n", PACKET_SIZE + sizeof(struct icmphdr), g_ping.ip, icmp.icmp_seq, ip->ip_ttl, latency);

        g_ping.total_time += latency;
        if (g_ping.received == 0 || latency < g_ping.min_rtt) g_ping.min_rtt = latency;
        if (g_ping.received == 0 || latency > g_ping.max_rtt) g_ping.max_rtt = latency;
        g_ping.sum_rtt += latency;
        if (g_ping.received < MAX_LATENCIES) g_ping.latencies[g_ping.received] = latency;
        g_ping.received++;

        if (g_ping.received > 1) {
            double mean = g_ping.sum_rtt / g_ping.received;
            double squared_diff_sum = 0.0;
            for (int i = 0; i < g_ping.received; i++)
                squared_diff_sum += (g_ping.latencies[i] - mean) * (g_ping.latencies[i] - mean);
            g_ping.mdev_rtt = squared_diff_sum / g_ping.received;
        }
        usleep(100000);
    }
}

