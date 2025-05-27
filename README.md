# 📸 SVD Image Compressor · C

> _Tiny CLI utility that shrinks grayscale images via **truncated Singular Value Decomposition**._

| ![cool dog color](/images/dog/cool-dog-color.gif) | ![cool dog no color](/images/dog/cool-dog-nocolor.gif) |
|:-------------------------------------------------:|:------------------------------------------------------:|

---

## 🧠 TL;DR SVD in one minute

Any matrix $A \in \mathbb{R}_{m \times n}$ can be decomposed as

$$
A = U \Sigma V^{T},
$$

where $U$ and $V$ are orthogonal matrices and $\Sigma$ is diagonal.  
If you keep only the first $k$ singular triplets you get the best rank-k approximation (*in Frobenius norm*):

$$
A_k = \sum_{r=1}^{k} \sigma_r u_r v_{r}^{T}, \qquad k \ll \min(m,n)
$$

Treat the pixels of a grayscale image as entries of $A$ and you obtain **huge size reduction** with **graceful quality
loss**.

<details>
<summary>📐 More maths (click to expand) </summary>

* Power iteration finds the dominant singular vector by alternating multiplication with $A$ and $A^{T}$.
* After each triplet $(u_r, \sigma_r, v_r)$ we _deflate_ $A ← A − u_r \sigma_r v_{r}^{T}$ to expose the next $\sigma$.
* This process repeats until the requested rank is reached.

</details>

---

## 🚀 Quick start

```bash
# 1 – Install ImageMagick (macOS example)
brew install imagemagick

# 2 – Build (out-of-tree)
mkdir -p build && cd build
cmake ..
make

# 3 – Compress!
./svd <input> <output.pgm> <rank>
```

Example:

```bash
./svd --color=0 ../images/dog/input.png ../images/dog/color-nocolor/nocolor/output-nocolor-4.pgm 4  
# if you want RGB image

./svd --color=1 ../images/dog/input.png ../images/dog/color-nocolor/color/output-color-4.ppm 4  
#if you want BW image

# → subsequent runs are near-instant: “loaded 4 from cache ...”
```

---

## 🗂 Project layout

```text
.
├── build/            # here you can build                  
├── images/           # here you can load images and try SVD
├── include/          # public headers                      
│   ├── cache.h                                             
│   ├── convert.h                                           
│   ├── pgm.h                                               
│   └── svd.h                                               
├── src/              # implementation                      
│   ├── cache.c  ─┐   # cache I/O                           
│   ├── convert.c │   # ImageMagick call                    
│   ├── pgm.c     │   # PGM parser/writer                   
│   ├── svd.c     │   # SVD                                 
│   └── main.c    ┘   # CLI                                 
├── CMakeLists.txt
└── README.md
```