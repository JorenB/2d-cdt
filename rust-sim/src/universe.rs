use crate::vertex::Vertex;
use crate::triangle::{Triangle, TriType};

pub struct Universe {
    pub n_slices: usize,
    pub vertices: Vec<Vertex>,
    pub triangles: Vec<Triangle>,
}

impl Universe {
    pub fn new(n_slices: usize) -> Self {
        let w = 3usize;
        let mut vertices = Vec::new();
        for i in 0..n_slices {
            for _ in 0..w {
                let id = vertices.len();
                vertices.push(Vertex { id, time: i });
            }
        }
        let mut triangles = Vec::new();
        for i in 0..n_slices {
            for j in 0..w {
                let vl = i * w + j;
                let vr = i * w + (j + 1) % w;
                let vc = ((i + 1) % n_slices) * w + j;
                let id = triangles.len();
                triangles.push(Triangle { id, vl, vr, vc, typ: TriType::Up });
                let vl2 = ((i + 1) % n_slices) * w + j;
                let vr2 = ((i + 1) % n_slices) * w + (j + 1) % w;
                let vc2 = i * w + (j + 1) % w;
                let id2 = triangles.len();
                triangles.push(Triangle { id: id2, vl: vl2, vr: vr2, vc: vc2, typ: TriType::Down });
            }
        }
        Self { n_slices, vertices, triangles }
    }

    pub fn volume(&self) -> usize {
        self.triangles.len()
    }
}
