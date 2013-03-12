#ifndef _MY_FUNC_
#define _MY_FUNC_

#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <linux/ip.h>

#define SADDR 1
#define DADDR 2

struct iphdr *network_header;
struct ethhdr *eth;
__be32 ip_address;
static u32 timestamp; 
static u32 now;
static u32 rtt;
static int cpu;

static inline void print_kernel_emerg (char* msg) {
	printk(KERN_EMERG "%s", msg);
}

static inline void print_ip_addr (struct sk_buff *skb, unsigned char verse) {
 	network_header = (struct iphdr *)skb_network_header(skb);
 	if(network_header != NULL) {
		switch (verse) {
			case SADDR:
                printk(KERN_EMERG "%pI4", &network_header->saddr);
                return;
			case DADDR:
                printk(KERN_EMERG "%pI4", &network_header->daddr);
                return;
            default:
                return;
         }
 	}
}

static inline void print_mac_addr (struct sk_buff *skb, unsigned char verse) {
 	eth = (struct ethhdr*)skb_mac_header(skb);
 	if(eth != NULL) {
		switch (verse) {
			case SADDR:
                printk(KERN_EMERG "%pI4", &eth->h_source);
                return;
			case DADDR:
                printk(KERN_EMERG "%pI4", &eth->h_dest);
                return;
            default:
                return;
         }
 	}
}

																						//0x0101A8C0; //192.168.1.1
static inline void print_ip_addr_if_equal_to (struct sk_buff *skb, unsigned char verse, __be32 _ip_address) {
 	network_header = (struct iphdr *)skb_network_header(skb);
 	if(network_header != NULL) {
 		if (network_header->daddr == _ip_address) {
			switch (verse) {
				case SADDR:
            		printk(KERN_EMERG "%pI4", &network_header->saddr);
					return;
				case DADDR:
                	printk(KERN_EMERG "%pI4", &network_header->daddr);
                	return;
            	default:
                	return;
         	}
 		}
	}
}
														//0x0101A8C0; //192.168.1.1
static inline void rtt_start (struct sk_buff *skb, __be32 _ip_address) {
	preempt_disable();
	cpu = smp_processor_id();
	rdtsc_barrier();
	rdtscl(timestamp);
	/* Allow RT tasks to run */
	preempt_enable();
	network_header = (struct iphdr *)skb_network_header(skb);
	if (network_header != NULL)
		if (network_header->protocol == 1)	//ICMP
			if (network_header->daddr == _ip_address)
				printk(KERN_EMERG "\nRTT START: sent to %pI4 (packet type: ICMP) timestamp: %d\n", &network_header->daddr, timestamp);
}

														//0x0101A8C0; //192.168.1.1
static inline void rtt_stop (struct sk_buff *skb, __be32 _ip_address) {
	preempt_disable();
	cpu = smp_processor_id();
	rdtsc_barrier();
	rdtscl(now);
	rtt = timestamp - now;
 	/*
 	* It is possible that we moved to another CPU, and
	* since TSC's are per-cpu we need to calculate
	* that. The delay must guarantee that we wait "at
	* least" the amount of time. Being moved to another
	* CPU could make the wait longer but we just need to
	* make sure we waited long enough. Rebalance the
	* counter for this CPU.
	*/
	if (unlikely(cpu != smp_processor_id())) {
		rtt += (now - timestamp);
	}
	preempt_enable();
	network_header = (struct iphdr *)skb_network_header(skb);
	if (network_header != NULL)
		if (network_header->protocol == 1)	//ICMP
			if (network_header->saddr == _ip_address)
				printk(KERN_EMERG "RTT STOP: sent from %pI4 (packet type: ICMP) rtt: %d\n", &network_header->saddr, rtt);
}
	
#endif