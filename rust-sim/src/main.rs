mod vertex;
mod triangle;
mod universe;
mod simulation;
mod config;

use simulation::Simulation;
use config::Config;
use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();
    let filename = args.get(1).cloned().unwrap_or_else(|| "config.dat".to_string());
    let cfg = Config::from_file(&filename).expect("config file");
    let lambda: f64 = cfg.get("lambda");
    let target_volume: usize = cfg.get("targetVolume");
    let slices: usize = cfg.get("slices");
    let seed: u64 = cfg.get("seed");
    let measurements: usize = cfg.get("measurements");

    let mut sim = Simulation::new(lambda, target_volume, seed);
    sim.start(measurements, slices);
}
