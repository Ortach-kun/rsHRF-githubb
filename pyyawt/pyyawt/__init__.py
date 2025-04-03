from ._pyyawt import dwt, idwt

__version__ = '0.1.0'
__all__ = ['dwt', 'idwt']

def wavelet_transform(signal):
    """
    Perform discrete wavelet transform on input signal
    
    Parameters
    ----------
    signal : numpy.ndarray
        Input signal to transform
        
    Returns
    -------
    numpy.ndarray
        Wavelet coefficients
    """
    return dwt(signal)

def inverse_wavelet_transform(coeffs):
    """
    Perform inverse discrete wavelet transform
    
    Parameters
    ----------
    coeffs : numpy.ndarray
        Wavelet coefficients
        
    Returns
    -------
    numpy.ndarray
        Reconstructed signal
    """
    return idwt(coeffs) 