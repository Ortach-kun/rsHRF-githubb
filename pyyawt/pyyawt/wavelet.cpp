#include <vector>
#include <cmath>
#include <complex>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// Haar wavelet transform (iterative version)
void haar_transform(std::vector<double>& data) {
    size_t length = data.size();
    std::vector<double> temp(length);
    
    while (length > 1) {
        size_t half = length / 2;
        
        for (size_t i = 0; i < half; i++) {
            size_t k = i * 2;
            temp[i] = (data[k] + data[k + 1]) / std::sqrt(2.0);
            temp[half + i] = (data[k] - data[k + 1]) / std::sqrt(2.0);
        }
        
        for (size_t i = 0; i < length; i++) {
            data[i] = temp[i];
        }
        
        length = half;
    }
}

// Inverse Haar wavelet transform (iterative version)
void inverse_haar_transform(std::vector<double>& data) {
    size_t length = 2;
    std::vector<double> temp(data.size());
    
    while (length <= data.size()) {
        size_t half = length / 2;
        
        for (size_t i = 0; i < half; i++) {
            double a = data[i];
            double b = data[half + i];
            temp[i * 2] = (a + b) / std::sqrt(2.0);
            temp[i * 2 + 1] = (a - b) / std::sqrt(2.0);
        }
        
        for (size_t i = 0; i < length; i++) {
            data[i] = temp[i];
        }
        
        length *= 2;
    }
}

// Wrapper function for numpy arrays
py::array_t<double> dwt(py::array_t<double> input) {
    py::buffer_info buf = input.request();
    if (buf.ndim != 1) {
        throw std::runtime_error("Number of dimensions must be 1");
    }
    
    double *ptr = static_cast<double *>(buf.ptr);
    std::vector<double> signal(ptr, ptr + buf.size);
    
    // Ensure signal length is power of 2
    size_t original_size = signal.size();
    size_t padded_size = 1;
    while (padded_size < original_size) {
        padded_size *= 2;
    }
    signal.resize(padded_size, 0.0);
    
    // Perform transform
    haar_transform(signal);
    
    // Trim back to original size
    signal.resize(original_size);
    
    auto output = py::array_t<double>(original_size);
    py::buffer_info buf_out = output.request();
    double *ptr_out = static_cast<double *>(buf_out.ptr);
    std::copy(signal.begin(), signal.end(), ptr_out);
    
    return output;
}

// Wrapper function for inverse transform
py::array_t<double> idwt(py::array_t<double> input) {
    py::buffer_info buf = input.request();
    if (buf.ndim != 1) {
        throw std::runtime_error("Number of dimensions must be 1");
    }
    
    double *ptr = static_cast<double *>(buf.ptr);
    std::vector<double> coeffs(ptr, ptr + buf.size);
    
    // Ensure coefficients length is power of 2
    size_t original_size = coeffs.size();
    size_t padded_size = 1;
    while (padded_size < original_size) {
        padded_size *= 2;
    }
    coeffs.resize(padded_size, 0.0);
    
    // Perform inverse transform
    inverse_haar_transform(coeffs);
    
    // Trim back to original size
    coeffs.resize(original_size);
    
    auto output = py::array_t<double>(original_size);
    py::buffer_info buf_out = output.request();
    double *ptr_out = static_cast<double *>(buf_out.ptr);
    std::copy(coeffs.begin(), coeffs.end(), ptr_out);
    
    return output;
}

PYBIND11_MODULE(_pyyawt, m) {
    m.doc() = "C++ implementation of wavelet transforms";
    m.def("dwt", &dwt, "Discrete Wavelet Transform");
    m.def("idwt", &idwt, "Inverse Discrete Wavelet Transform");
} 