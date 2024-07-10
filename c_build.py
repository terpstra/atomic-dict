from setuptools import setup, Extension, find_packages

atomic_dict_c_module = Extension(
    "atomic_dict.atomic_dict_c",
    sources=["c/init.c", "c/methods.c"],
    extra_compile_args=["-O3"]
)

setup(
    name="atomic_dict",
    version="0.2.0",
    packages=find_packages(),
    ext_modules=[atomic_dict_c_module],
)
