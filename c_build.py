import os
from setuptools import setup, Extension, find_packages # type: ignore
import sys

cmd = ['build', '--build-lib', os.getcwd()]
sys.argv.extend(cmd)

atomic_dict_capi_module = Extension(
    "atomic_dict.capi",
    sources=["c/init.c", "c/methods.c"],
    extra_compile_args=["-O3"]
)

setup(
    name="atomic_dict",
    version="0.4.0",
    packages=find_packages(),
    ext_modules=[atomic_dict_capi_module],
)
