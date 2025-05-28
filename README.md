# 📸 SVD Image Compressor · C

> _Tiny CLI utility that shrinks images via **truncated Singular Value Decomposition**._

| ![cool dog color](/images/dog/cool-dog-color.gif) | ![cool dog no color](/images/dog/cool-dog-nocolor.gif) |
|:-------------------------------------------------:|:------------------------------------------------------:|

---

## 🧠 TL;DR SVD in one minute

Any real matrix $A \in Mat_{m \times n}(\mathbb R)$ admits a decomposition

$$
A = U \Sigma V^{T}
$$

where

- $U \in Mat_{m \times m}(\mathbb R)$ and $V \in Mat_{n \times n}(\mathbb R)$ are **orthogonal** ($U U^{T} = E$
  and $V V^{T} = E$)
- $\Sigma = diag(\sigma_{1}, ..., \sigma_{r})$ with $r = min(m, n)$ and **singular values** $\sigma_1 \ge \sigma_2 \ge \dots \ge \sigma_r \ge 0$

If we keep only the top $k$ singular values and corresponding vectors

$$
A_{k} = U_{k} \Sigma_{k} V_{k}^{T} = \sum_{i=1}^{k} \sigma_{i} u_{i} v_{i}^{T}
$$

$A_k$ will be the best *rank-k approximation* to $A$ in the **Frobenius norm**.

Directly computing a full SVD is $O(c \times m n \times min(m,n))$, which can be expensive for large images. Instead, we can extract the top $k$ singular triplets iteratively using power iteration and deflation:

Let $A^{(0)} = A$. For each $i = 1 \dots k$:

1. initialise a random unit vector $v^{(0)} \in \mathbb R^{n}$

2. for $t = 1, \dots, T$:

$$
w^{(t)} = A^{(i-1)} v^{(t-1)}, \qquad w^{(t)} \leftarrow \frac{w^{(t)}}{||w^{(t)}||}
$$

$$
z^{(t)} = (A^{(i-1)})^{T} w^{(t)}, \qquad v^{(t)} \leftarrow \frac{z^{(t)}}{||z^{(t)}||}
$$

3. after T steps set:

$$
u_{i} = w^{(T)}, \qquad v_{i} = v^{(T)}, \qquad \sigma_{i} = ||A^{(i-1)} v_{i}||
$$

4. remove the contribution of the extracted singular triplet from the working matrix:
   
$$
A^{(i)} \leftarrow A^{(i-1)} - \sigma_{i} u_{i} v_{i}^{T}
$$

Algorithm complexity: $O(c \times k \times (T+1) \times m n)$, where:

- $c$ — number of channels (1 for grayscale, 3 for RGB)
- $k$ — target rank
- $T$ — number of power‐iteration steps per component (default `T = 100`)
- $m, n$ — image dimensions: height (rows) and width (columns)

With $T$ treated as a constant, complexity reduces to $O(c \times k \times m n)$.

---

## 🚀 Quick start

```bash
# 1 – Install ImageMagick (macOS example)
MacBook :: ((somewhere)) >> brew install imagemagick

# 2 – Build
MacBook :: .../SVD       >> mkdir build && cd build
MacBook :: .../SVD/build >> cmake ..
MacBook :: .../SVD/build >> make

# 3 – Compress!
MacBook :: .../SVD/build >> ./svd [--color=0] [--cache=0] <input> <output> <rank>
```

Example (for this repo):

```bash
MacBook :: .../SVD/build >> ./svd --color=0 ../images/dog/input.png ../images/dog/color-nocolor/nocolor/output-nocolor-4 4  
# if you want BW image

MacBook :: .../SVD/build >> ./svd --color=1 ../images/dog/input.png ../images/dog/color-nocolor/color/output-color-4 4  
#if you want RGB image

# → subsequent runs are near-instant: “loaded 4 from cache ...”
```

---

## 🗂 Project layout

```text
.
├── + build/                  # here you can build (follow Quick start)
├── images/                   # here you can load images and try SVD
├── include/                  # public headers
│   ├── cache/                # cache I/O API
│   │   └── cache.h
│   ├── core/                 # processing
│   │   ├── cli.h             # reading args from cmd
│   │   ├── processor.h       # main worker
│   │   └── utils.h           # utilities
│   ├── image-processor/      # image module
│   │   ├── pgm-ppm-parsers/  # PGM/PPM parsers
│   │   │   ├── pgm.h
│   │   │   └── ppm.h
│   │   ├── convert.h         # any image to PGM or PPM
│   │   └── image.h           # image interface
│   ├── linear-algebra/       # SVD
│   │   └── svd.h
├── src/                      # implementations
│   ├── cache/
│   ├── core/
│   ├── image-processor/
│   ├── linear-algebra/
│   └── main.c                # CLI
├── CMakeLists.txt
└── README.md
```
