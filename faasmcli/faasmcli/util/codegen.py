from faasmcli.util.env import POSSIBLE_BUILD_BINS
from faasmcli.util.shell import find_command


def find_codegen_shared_lib():
    return find_command("codegen_shared_obj", POSSIBLE_BUILD_BINS)


def find_codegen_func(wamr=False):
    if wamr:
        return find_command("wamrc", POSSIBLE_BUILD_BINS)
    else:
        return find_command("codegen_func", POSSIBLE_BUILD_BINS)
