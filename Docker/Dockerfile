# QuasiDipole: Docker/Dockerfile

# Copyright (C) 2023  Johnathan K Burchill

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

FROM johnathanburchill/utils:cdev AS dev
USER science
ENTRYPOINT ["/bin/bash"]

FROM dev as build
WORKDIR /home/science/build
USER science
RUN git clone https://github.com/JohnathanBurchill/QuasiDipole
WORKDIR /home/science/build/QuasiDipole
RUN mkdir build
WORKDIR /home/science/build/QuasiDipole/build
RUN cmake ..
RUN make 
ENTRYPOINT ["/bin/bash"]

FROM scratch AS deploy
COPY --from=build /home/science/build/QuasiDipole/build/qdexample /.
COPY --from=build /home/science/build/QuasiDipole/include/quasidipole.h /.
COPY --from=build /home/science/build/QuasiDipole/build/libquasidipole.a /.
ENTRYPOINT ["/quasidipole"]

