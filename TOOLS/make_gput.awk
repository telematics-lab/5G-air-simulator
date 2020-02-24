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

BEGIN {
    TransportBlockSize[0] = 0
    TransportBlockSize[1] = 18
    TransportBlockSize[2] = 23
    TransportBlockSize[3] = 28
    TransportBlockSize[4] = 37
    TransportBlockSize[5] = 45
    TransportBlockSize[6] = 59
    TransportBlockSize[7] = 72
    TransportBlockSize[8] = 89
    TransportBlockSize[9] = 105
    TransportBlockSize[10] = 123
    TransportBlockSize[11] = 141
    TransportBlockSize[12] = 159
    TransportBlockSize[13] = 177
    TransportBlockSize[14] = 203
    TransportBlockSize[15] = 230
    TransportBlockSize[16] = 259
    TransportBlockSize[17] = 289
    TransportBlockSize[18] = 288
    TransportBlockSize[19] = 308
    TransportBlockSize[20] = 328
    TransportBlockSize[21] = 363
    TransportBlockSize[22] = 399
    TransportBlockSize[23] = 433
    TransportBlockSize[24] = 468
    TransportBlockSize[25] = 506
    TransportBlockSize[26] = 543
    TransportBlockSize[27] = 578
    TransportBlockSize[28] = 614
    TransportBlockSize[29] = 640
    TransportBlockSize[30] = 667
    TransportBlockSize[31] = 0
}

/^PHY_RX/ {
    mcs = $15
    n_rb = $13
    goodput += n_rb * TransportBlockSize[mcs]
}

/^TX/ {
    time = $14
}

END{
    print goodput / time
}

