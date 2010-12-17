void __stop_progExec__ (void) {
#ifndef NOCPUOFF
    _BIS_SR(CPUOFF);
#endif // NOCPUOFF
}
