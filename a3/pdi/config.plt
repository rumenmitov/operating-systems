set terminal png
set output "results/output.png"

set title "Time Taken for a Certain Amount of Threads to Complete the Execution of the PDI Program"

set xlabel "Thread Count"
set ylabel "Time Taken, s"

set style data lines

plot "results/time.dat" title "Time Taken for Certain Amount of Threads"
