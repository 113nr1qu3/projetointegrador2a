cmake_minimum_required(VERSION 3.10)
project(projetointegrador2a)

# Busca recursiva em todas as subpastas
file(GLOB_RECURSE SOURCES "projeto integrador/*.c")

add_executable(meu_app ${SOURCES})