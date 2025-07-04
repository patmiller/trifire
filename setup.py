from setuptools import setup, Extension
import os

my_module_extension = Extension(
    'trifire',
    sources=['src/trifire.c',
             'ball.c',
	     'bmp.c',
	     'bricks.c',
	     'coin.c',
             'explosion.c',
	     'penrose.c',
	     'penrosecoin.c',
	     'play.c',
	     'render.c',
	     'sha1.c',
	     'state.c',
             'trajectory.c',
    ],
    include_dirs = [os.getcwd()],
)

setup(
    name='trifire',
    version='1.0',
    description='trifire game engine',
    ext_modules=[my_module_extension]
)
