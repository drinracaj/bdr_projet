# when cloning the repo

- git clone ..
- git submodule update --init --recursive to dl the soci repo


# requierement
- cmake
- compilateur c++ avec c++20
- make
- libpq (library cliente de postgresql)
- qt6

# step pour setup
- clone l'app
- docker-compose up -d
- depuis la racine du projet sudo docker exec -i bdr_postgresql_16 psql -U postgres -d nombd < init/01-init.sql (Widows : docker exec -i bdr_postgresql_16 psql -U postgres -d nombd < init/01-init.sql)
- depuis la racine du projet faire : mkdir /build && cd build
- faire: cmake ..
- faire: make
- le build va prendre un peu de temps ..
- lancer l'application se nommant rjmmo et se trouvant dans /build