These source codes are used to verify the theoretically obtained results on AES by working on a small scale AES. These results are published in our paper at SAC 2020 with the title "Weak-Key Distinguishers for AES".

Weak-Key Subspace Trails and Applications to AES
------- EXPERIMENTS using Small-Scale AES -------

Our simplified C implementation is provided in "small_aes.c" file. This implementation can be run both on Linux and Windows (Visual Studio). "small_aes.h" library only contains the matrix multiplication and its inverse as arrays which can be used to speed up the experiments. Decryption operation uses the provided array but for this reference implementation encryption operation uses a normal implementation of matrix multiplication.

The code initially receives the following choices from the user:
1) Encryption or Decryption
2) Key
3) Number of rounds

Then it encrypts (decrypts) the invariant subspace of 2^32 texts and provides the number of collisions in the anti-diagonals (diagonals) which should be checked for the multiple-of-n property that is used in the construction of the distinguishers we provided in the paper.

These codes can be used to verify 10-round and 12-round chosen-key distinguishers that we proposed in the paper. The implementation uses Small-scale AES to encrypt or decrypt the invariant subspace containing 2^32 texts for any number of rounds. It allows both 64-bit and 128-bit keys which correspond to AES-128 and AES-256. When IS (defined in the paper) is encrypted with a weak-key, multiple-of-128 and multiple-of-2 properties should hold for 5 and 6 rounds, respectively. And these results can be extended to 7 and 8 rounds when a suitable 128-bit key is selected as described in the paper. Similarly, same properties should hold for 4 and 5 rounds of decryption, respectively. 

Thus, observing the multiple-of-128 property for "e" rounds of encryption and "d" rounds of decryption allows us to construct "e + d"-round chosen-key distinguisher as explained in the paper. When a 128-bit key is used, our implementation uses both 64-bit parts of the key during decryption so the result provides "e + d - 1"-round chosen-key distinguisher. 

As we explained in the paper, there exists a key for which the multiple-of-n properties for the decryption hold for 4 and 5 rounds. This key is determined by the 4th round constant and S(0) where S is the S-box. For this small-scale AES this key is 0xe666e666e666e666. This validates the 10-round and 14-round chosen-key distinguishers and the output of the experiment with this key is provided below as an example.

Our full code which provides many other functionalities is provided in "small_aes_full.c" file. This implementation also allows to practically verify multiple-of-2 property on the X subspace. However, this implementation is done for Visual Studio and requires small modifications for other compilers.



-------------- Example Output ---------------------
(1) Small AES Invariant Subspace - Encryption Experiment
(2) Small AES Invariant Subspace - Encryption Experiment (128-bit key)
(3) Small AES Invariant Subspace - Decryption Experiment
(4) Small AES Invariant Subspace - Decryption Experiment (128-bit key)
Choice: 3
Number of rounds: 5
(0) 357a357a357a357a
(1) f903f903f903f903
(2) 0
(3) 5555555555555555
(4) Enter manually
Select key: 4
Enter key (e.g. 0123456789abcdef): e666e666e666e666
Selected key is: e666e666e666e666
Diagonal 1: 140735340871680
Diagonal 2: 140735340871680
Diagonal 3: 140735340871680
Diagonal 4: 140735340871680
Time: 827 seconds
