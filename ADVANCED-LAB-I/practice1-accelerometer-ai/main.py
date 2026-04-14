from scripts.python.model import run_model_pipeline
from scripts.python.save_data import run_data_collection

if __name__ == "__main__":
    print("Starting data collection...")
    run_data_collection()

    print("Starting model training and conversion...")
    run_model_pipeline()

    print("Pipeline completed successfully!")