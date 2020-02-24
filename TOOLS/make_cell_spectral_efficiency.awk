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
# Author: Giovanni Civitano
# Author: Alessandro Grassi
# Author: Giuseppe Milella
# Author: Micaela Perrone
#

BEGIN {
    if (bw == 0) {
        print "Usage: make_cell_spectral_efficiency -v bw=<bandwidth_in_MHz> simulation_output"
        exit 1
    }
}

/^RX/ {
    goodput += $8
}

/^TX/ {
    time = $14
}

END {
    if (bw != 0) {
        print (goodput*8) / time / (bw*1000000)
    }
}

