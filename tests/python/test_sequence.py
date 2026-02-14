import unittest
from cbits import BitVector

class TestSequenceOps(unittest.TestCase):
    def test_concat_basic(self):
        a = BitVector(5)
        b = BitVector(3)

        a.set(0); a.set(2); a.set(4)
        b.set(1)

        c = a + b
        self.assertIsInstance(c, BitVector)
        self.assertEqual(len(c), 8)

        expected = [1,0,1,0,1, 0,1,0]
        self.assertEqual(expected, [c.get(i) for i in range(8)])

    def test_concat_unaligned(self):
        a = BitVector(70)
        b = BitVector(70)

        a.set(0); a.set(69)
        b.set(1); b.set(68)

        c = a + b
        self.assertEqual(len(c), 140)
        self.assertTrue(c.get(0))
        self.assertTrue(c.get(69))
        self.assertTrue(c.get(70 + 1))
        self.assertTrue(c.get(70 + 68))

    def test_concat_type_error(self):
        a = BitVector(5)
        with self.assertRaises(TypeError):
            _ = a + 123

    def test_repeat_basic(self):
        a = BitVector(4)
        a.set(0); a.set(3)

        r = a * 3
        self.assertEqual(len(r), 12)

        expected = [1,0,0,1] * 3
        self.assertEqual(expected, [r.get(i) for i in range(12)])

    def test_repeat_zero(self):
        a = BitVector(10)
        r = a * 0
        self.assertEqual(len(r), 0)

    def test_repeat_negative(self):
        a = BitVector(5)
        with self.assertRaises(ValueError):
            _ = a * -1

    def test_repeat_left_mul(self):
        a = BitVector(3)
        a.set(1)

        r = 4 * a
        self.assertEqual(len(r), 12)
        expected = [0,1,0] * 4
        self.assertEqual(expected, [r.get(i) for i in range(12)])

if __name__ == "__main__":
    unittest.main()
