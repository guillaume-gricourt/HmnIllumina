import subprocess
import tempfile

from tests import main_tests


class TestReproducibility(main_tests.MainTests):
    def test_run(self):
        with tempfile.NamedTemporaryFile() as fd:
            args = [self.exec_path]
            args += ["--input", self.illumina_one]
            args += ["--output", fd.name]
            ret = main_tests.MainTests.cmd(args=args)
            self.assertTrue(
                main_tests.MainTests.compare_json(one=fd.name, two=self.res_one)
            )
        with tempfile.NamedTemporaryFile() as fd:
            args = [self.exec_path]
            args += ["-i", self.illumina_one]
            args += ["-o", fd.name]
            ret = main_tests.MainTests.cmd(args=args)
            self.assertTrue(
                main_tests.MainTests.compare_json(one=fd.name, two=self.res_one)
            )
