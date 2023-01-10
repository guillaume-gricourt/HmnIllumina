import json
import logging
import os
import subprocess
import unittest
from typing import Any, Dict, List


class MainTests(unittest.TestCase):
    dataset_path = os.path.join(os.path.dirname(__file__), "dataset")
    exec_path = os.path.join(os.path.dirname(__file__), "HmnIllumina")
    # Gold input
    illumina_path = os.path.join(dataset_path, "illumina")
    illumina_one = os.path.join(illumina_path, "200101_M999_0999_000000000-Z9ZZ9")
    illumina_one_interop = os.path.join(illumina_one, "InterOp")
    illumina_one_info = os.path.join(illumina_one, "RunInfo.xml")
    illumina_one_parameter = os.path.join(illumina_one, "RunParameters.xml")
    # Gold output
    res_path = os.path.join(dataset_path, "result")
    res_one = os.path.join(res_path, "200101_M999.json")

    @classmethod
    def cmd(
        cls, args: List[str], show_output: bool = True
    ) -> subprocess.CompletedProcess:
        """Run a command line.

        Parameters
        ----------
        args: List[str]
            A list of argument
        show_output: bool (default: True)
            Output command line

        Return
        ------
        subprocess.CompletedProcess
            Return result obtained with subprocess
        """
        code = subprocess.run(args, capture_output=True, encoding="utf8")
        if show_output and code.stdout is not None:
            logging.info(code.stdout)
        if show_output and code.stderr is not None:
            logging.warning(code.stderr)
        return code

    @classmethod
    def load_json(cls, path: str) -> Dict[Any, Any]:
        with open(path) as fid:
            return json.load(fid)

    @classmethod
    def compare_json(cls, one: str, two: str) -> bool:
        first = cls.load_json(path=one)
        second = cls.load_json(path=two)
        return first == second
