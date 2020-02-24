#!/usr/bin/awk -f
#
# Copyright (c) 2018 TELEMATICS LAB, Politecnico di Bari
#
# This file is part of 5G-air-simulator
#
# 5G-air-simulator is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 as
# published by the Free Software Foundation;
#
# 5G-air-simulator is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with 5G-air-simulator; if not, see <http://www.gnu.org/licenses/>.
#
# Author: Alessandro Grassi <alessandro.grassi@poliba.it>
#

/^TX/ {
    time = $14
}

/^RX/ {
    node = $12
    goodput[node] += $8 * 8
}

END{
    for (node in goodput) {
        sum_goodput += goodput[node]/time
        sum_quad_goodput += (goodput[node]/time)^2
        n += 1
    }
    quad_sum_goodput = sum_goodput^2
    avg_goodput = sum_goodput/n
    fi = quad_sum_goodput/(n*sum_quad_goodput)
    printf("tot %2.9lf\n",sum_goodput);
    printf("avg %2.9lf\n",avg_goodput);
    printf("FI %2.9lf\n",fi);
}

