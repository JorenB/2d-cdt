use std::collections::HashMap;
use std::fs;
use std::str::FromStr;

pub struct Config {
    data: HashMap<String, String>,
}

impl Config {
    pub fn from_file(path: &str) -> std::io::Result<Self> {
        let content = fs::read_to_string(path)?;
        let mut data = HashMap::new();
        for line in content.lines() {
            let mut parts = line.split_whitespace();
            if let (Some(k), Some(v)) = (parts.next(), parts.next()) {
                data.insert(k.to_string(), v.to_string());
            }
        }
        Ok(Self { data })
    }

    pub fn get<T: FromStr>(&self, key: &str) -> T {
        self.data.get(key).expect("missing key").parse().ok().expect("parse")
    }
}
