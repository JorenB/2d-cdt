#[derive(Clone)]
pub enum TriType {
    Up,
    Down,
}

#[derive(Clone)]
pub struct Triangle {
    pub id: usize,
    pub vl: usize,
    pub vr: usize,
    pub vc: usize,
    pub typ: TriType,
}
