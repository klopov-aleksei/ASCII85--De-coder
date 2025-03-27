import unittest
import subprocess
import base64
import random
import os

EXECUTABLE = "./ascii85" # build

def run_ascii85(args, input_data):
    process = subprocess.run([EXECUTABLE] + args,
                          input=input_data,
                          stdout=subprocess.PIPE,
                          stderr=subprocess.PIPE)
    return process

def normalize_output(out_bytes):
    return out_bytes.rstrip(b"\r\n")

class TestASCII85(unittest.TestCase):
    def generate_random_data(self, size):
        return os.urandom(size)

    def test_encode_random_data(self):
        for _ in range(10):
            data = self.generate_random_data(random.randint(1, 128))
            expected = base64.a85encode(data).decode('ascii')
            proc = run_ascii85(["-e"], data)
            self.assertEqual(proc.returncode, 0, msg=f"Encoder завершился с ошибкой: {proc.stderr.decode()}")
            result = normalize_output(proc.stdout).decode('ascii')
            self.assertEqual(result, expected,
                             msg=f"Кодирование не совпадает.\nОжидаемый результат:\n{expected}\nПолучено:\n{result}")

    def test_decode_random_data(self):
        for _ in range(10):
            data = self.generate_random_data(random.randint(1, 128))
            encoded = base64.a85encode(data).decode('ascii')
            proc = run_ascii85(["-d"], encoded.encode('ascii'))
            self.assertEqual(proc.returncode, 0, msg=f"Decoder завершился с ошибкой: {proc.stderr.decode()}")
            self.assertEqual(normalize_output(proc.stdout), data,
                             msg="Декодирование не восстановило исходные данные.")

    def test_invalid_decode_data(self):
        invalid_inputs = [
            b"!!!!~",
            b"B",
            "Привет".encode(),
        ]
        for invalid in invalid_inputs:
            proc = run_ascii85(["-d"], invalid)
            self.assertNotEqual(proc.returncode, 0,
                                msg=f"Декодер должен завершаться с ошибкой на входе: {invalid.decode(errors='replace')}")

if __name__ == '__main__':
    unittest.main()