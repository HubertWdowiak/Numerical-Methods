set terminal post enhanced colour solid font 21 
set encoding iso_8859_2

set key spacing 2 samplen 3
set xlabel "x" 
set ylabel "f(x)"
set grid
x_min = 0.
x_max = 2.*pi
l_width = 5
p_size = 1.3

set style line 1 lt rgb "gray30" lw 6 pt 7 ps p_size
set style line 2 lt rgb "red" lw l_width pt 7 ps p_size
set style line 3 lt rgb "dark-spring-green" lw l_width pt 7 ps p_size
set style line 4 lt rgb "royalblue" lw l_width pt 7 ps p_size*1.2

# PLIK pkt.dat zawiera polozenia wezlow z wszystkich zadan:
#   - W pierwszej serii danych: wspolrzedne wezlow: x_i oraz y_i dla 1. punktu (dla f1(x)).
#   - Po dwoch pustych liniach: wspolrzedne wezlow dla 2. punktu.
#   - Po kolejnych dwoch pustych liniach: wspolrzedne wezlow dla 3. punktu.
#   - Ostatnia seria danych: wspolrzedne wezlow dla 4. punktu.


# 1. punkt: aproksymacja f1(x), alpha = 0.
# ZALOZENIE: W pliku approx1.dat znajduja sie dwie kolumny: x, F(x)

set output "approx1.eps"
set ylabel "f(x)"
set key inside
plot [x_min:x_max] "pkt.dat" i 0 ls 1 pt 7 t "f_1(x_i)",\
 "approx1.dat" i 0 w l ls 2 t "F_1(x), M_s=M_c=5"


# 2. punkt: aproksymacja f2(x).
# ZALOZENIE: W pliku approx2.dat znajduja sie dwie kolumny: x, F(x)

set output "approx2.eps"
set ylabel "f(x)"
set key inside
plot [x_min:x_max] "pkt.dat" i 1 ls 1 pt 7 t "f_2(x_i)",\
 "approx2.dat" i 0 w l ls 2 t "F_2(x), M_s=M_c=5"


# 3. punkt: aproksymacja f3(x).
# ZALOZENIE: W pliku approx3.dat znajduja sie dwie kolumny: x, F(x) z TRZEMA SERIAMI DANYCH:
#   1. seria danych: dla M_s = 5, M_c = 0.
#   Po dwoch pustych liniach 2. seria danych: M_s = M_c = 5
#   Po dwoch pustych liniach 3. seria danych: M_s = M_c = 10

set output "approx3.eps"
set ylabel "f(x)"
set key inside
plot [x_min:x_max] "pkt.dat" i 2 ls 1 pt 7 t "f_3(x_i)",\
 "approx3.dat" i 0 w l ls 2 t "F_3(x), M_s=5, M_c=0",\
 "approx3.dat" i 1 w l ls 3 t "F_3(x), M_s=M_c=5",\
 "approx3.dat" i 2 w l ls 4 t "F_3(x), M_s=M_c=10"


# 4. punkt: aproksymacja f1(x), alpha losowa.
# ZALOZENIE: W pliku approx4.dat znajduja sie dwie kolumny: x, F(x) z DWIEMA SERIAMI DANYCH:
#   1. seria danych: dla M_s = 5, M_c = 5.
#   Po dwoch pustych liniach 2. seria danych: M_s = M_c = 30

set output "approx4.eps"
set ylabel "f(x)"
set key inside
plot [x_min:x_max] "pkt.dat" i 3 ls 1 pt 7 t "f_1(x_i), {/Symbol a} losowe",\
 "approx4.dat" i 0 w l ls 2 t "F_4(x), M_s=M_c=5",\
 "approx4.dat" i 1 w l ls 3 t "F_4(x), M_s=M_c=30"


# 4. punkt - cd., wykresy wspolczynnikow a i b.
# ZALOZENIE: W pliku ab4.dat znajduja sie trzy kolumny: i, a[i], b[i].
#    1. seria danych: dla M_s = M_c = 5.
#    Po dwoch pustych liniach 2. seria danych: dla M_s = M_c = 30.

set output "approx4_ab5.eps"
set xlabel "indeks i"
set ylabel "a_i, b_i"
plot "ab4.dat" i 0 u 1:2 w lp ls 2 pt 7 t "a_i, M_s = M_c = 5",\
 "" i 0 u 1:3 w lp ls 3 pt 7 t "b_i, M_s = M_c = 5"

set output "approx4_ab30.eps"
plot "ab4.dat" i 1 u 1:2 w lp ls 2 pt 7 t "a_i, M_s = M_c = 30",\
 "" i 1 u 1:3 w lp ls 3 pt 7 t "b_i, M_s = M_c = 30"
