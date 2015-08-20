
- queue execute:p1(0) -> p2(1) -> p2(2) -> p2(3) -> p2(4) -> p4
- queue wait:p3
- queue all:p1 p2 p3 p4 p5 

- if the job in the queue all arrive_t <= current_t
    - then put in the wait queue
- else:
    - //remain in the queue all
    
- in the priority queue wait
    - see whether the top node's arrive time is earlier than the top of queue execute
        - if so
            - see whether the burst time is smaller than burst time of the queue execute
            - if so
                - update the burst time of the top of the execute queue and pop it out, store in the queue wait
                - pop the top of the queue wait out and push into the execute queue