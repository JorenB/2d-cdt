use rand::rngs::StdRng;
use rand::{Rng, SeedableRng};
use crate::universe::Universe;

pub struct Simulation {
    lambda: f64,
    target_volume: usize,
    rng: StdRng,
}

impl Simulation {
    pub fn new(lambda: f64, target_volume: usize, seed: u64) -> Self {
        Self { lambda, target_volume, rng: StdRng::seed_from_u64(seed) }
    }

    pub fn start(&mut self, measurements: usize, slices: usize) {
        let mut universe = Universe::new(slices);
        for i in 0..measurements {
            self.sweep(&mut universe);
            println!("m {} volume {}", i, universe.volume());
        }
    }

    fn sweep(&mut self, universe: &mut Universe) {
        let moves = 100 * universe.volume();
        for _ in 0..moves {
            self.attempt_move(universe);
        }
    }

    fn attempt_move(&mut self, universe: &mut Universe) {
        let r: f64 = self.rng.gen();
        if r < 0.33 {
            self.move_add(universe);
        } else if r < 0.66 {
            self.move_delete(universe);
        } else {
            self.move_flip(universe);
        }
    }

    fn move_add(&mut self, _u: &mut Universe) {}
    fn move_delete(&mut self, _u: &mut Universe) {}
    fn move_flip(&mut self, _u: &mut Universe) {}
}
