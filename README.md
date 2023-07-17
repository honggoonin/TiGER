# TiGER: Tiny bandwidth key encapsulation mechanism for easy miGration based on RLWE(R)

This project contains the implementation and Known Answer Test (KAT) results of our KEM scheme, __TiGER__. This work is submitted to 'Korean Post-Quantum Cryptography Competition'(www.kpqc.or.kr).

## KAT
This directory contains KAT results and scripts to perform KAT of __TiGER128__, __TiGER192__, __TiGER256__.
In particular, ```run_KAT.sh``` repeats and updates KAT values based on latest reference implementation.  


## Update News
11/29/22 initial commit of reference implementation and KAT

02/21/23 v2.0 Update

07/17/23 v2.1 Update
- Fixed issue of public key and ciphertext size mismatch between specification document v2.0 and reference implementation of ciphertext and public key. (Resolve data type differences via bit/byte conversion encoding)
- Modified the implementation of storing indexes of the secret key (SK) according to the document to significantly reduce the size of SK and simplify the decapsulation operation.
- Updated based on Professor Hwajeong Seo's [KPQClean project](https://github.com/kpqc-cryptocraft/KPQClean) to enable easy comparison with other algorithm codes.
- Supplemented several side-channel attacks presented by Professor HeeSeok Kim (@6th KpqC workshop, July 14th).

   - Removed conditional statements from the HWT function (rejection sampling) to be executed in constant time.
   - Complemented the problem that the mask variable of D2 encoding is vulnerable to power analysis due to the Hamming weight difference (0 or -1).


## Citation
If your research employs our __TiGER__ scheme, please cite the following paper (https://eprint.iacr.org/2022/1651):
```
@misc{TiGER,
      author = {Seunghwan Park and Chi-Gon Jung and Aesun Park and Joongeun Choi and Honggoo Kang},
      title = {TiGER: Tiny bandwidth key encapsulation mechanism for easy miGration based on RLWE(R)},
      howpublished = {Cryptology ePrint Archive, Paper 2022/1651},
      year = {2022},
      note = {\url{https://eprint.iacr.org/2022/1651}},
      url = {https://eprint.iacr.org/2022/1651}
}
```
