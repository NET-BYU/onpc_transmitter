let Queue = {
    start: ffi('bool start_queue()'),
    put: ffi('bool queue_put(char *)'),
    peek: ffi('void queue_peek(void(*)(char *, userdata), userdata)'),
    remove: ffi('bool queue_delete()'),
};
